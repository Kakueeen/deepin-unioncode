/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     huangyu<huangyub@uniontech.com>
 *
 * Maintainer: huangyu<huangyub@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CLIENT_H
#define CLIENT_H

#include "common/lsp/protocol.h"
#include "common/lsp/newprotocol.h"

#include <QTcpSocket>
#include <QThread>

namespace lsp {

class ClientPrivate;
class Client : public QObject
{
    Q_OBJECT
    ClientPrivate *const d;
public:
    explicit Client(unsigned int port, const QString &host = "127.0.0.1");
    virtual ~Client();

    lsp::SemanticTokensProvider initSecTokensProvider();
    static bool exists(const QString &progrma);

    // textDocument/semanticTokens/full/delta
    void delta(const newlsp::SemanticTokensDeltaParams &params){}
    // textDocument/semanticTokens/full
    void full(const newlsp::SemanticTokensParams &params){}
    // textDocument/semanticTokens/range
    void range(const newlsp::SemanticTokensRangeParams &params){}

    // Notifications send to server
    // textDocument/didOpen
    void didOpen(const newlsp::DidOpenTextDocumentParams &params){}
    // textDocument/didChange
    void didChange(const newlsp::DidChangeTextDocumentParams &params){}
    // textDocument/willSave
    void willSave(const newlsp::WillSaveTextDocumentParams &params){}
    // textDocument/willSaveWaitUntil
    void willSaveWaitUntil(const newlsp::WillSaveTextDocumentParams &params){}
    // textDocument/didSave
    void didSave(const newlsp::DidSaveTextDocumentParams &params){}
    // textDocument/didClose
    void didClose(const newlsp::DidCloseTextDocumentParams &params){}

    // Requests
    // textDocument/declaration
    void declaration(const newlsp::DeclarationParams &params){}
    // textDocument/definition
    void definition(const newlsp::DefinitionParams &params){}
    // textDocument/typeDefinition
    void typeDefinition(const newlsp::TypeDefinitionParams &params){}
    // textDocument/implementation
    void implementation(const newlsp::ImplementationParams &params){}
    // textDocument/references
    void references(const newlsp::ReferenceParams &params){}
    // textDocument/prepareCallHierarchy
    void prepareCallHierarchy(const newlsp::CallHierarchyPrepareParams &params){}
    // textDocument/prepareTypeHierarchy
    void prepareTypeHierarchy(const newlsp::TypeHierarchyPrepareParams &params){}
    // textDocument/documentHighlight
    void documentHighlight(const newlsp::DocumentHighlightParams &params){}
    // textDocument/documentLink
    void documentLink(const newlsp::DocumentLinkParams &params){}
    // textDocument/hover
    void hover(const newlsp::HoverParams &params){}
    // textDocument/codeLens
    void codeLens(const newlsp::CodeLensParams &params){}
    // textDocument/foldingRange
    void foldingRange(const newlsp::FoldingRangeParams &params){}
    // textDocument/selectionRange
    void selectionRange(const newlsp::SelectionRangeParams &params){}
    // textDocument/documentSymbol
    void documentSymbol(const newlsp::DocumentSymbolParams &params){}
    // textDocument/inlayHint
    void inlayHint(const newlsp::InlayHintParams &params){}
    // textDocument/inlineValue
    void inlineValue(const newlsp::InlineValueParams &params){}
    // textDocument/moniker
    void moniker(const newlsp::MonikerParams &params){}
    // textDocument/completion
    void completion(const newlsp::CompletionParams &params){}
    // textDocument/diagnostic
    void diagnostic(const newlsp::DocumentDiagnosticParams &params){}
    // textDocument/signatureHelp
    void signatureHelp(const newlsp::SignatureHelpParams &params){}
    // textDocument/codeAction
    void codeAction(const newlsp::CodeActionParams &params){}
    // textDocument/documentColor
    void documentColor(const newlsp::DocumentColorParams &params){}
    // textDocument/colorPresentation
    void colorPresentation(const newlsp::ColorPresentationParams &params){}
    // textDocument/formatting
    void formatting(const newlsp::DocumentFormattingParams &params){}
    // textDocument/rangeFormatting
    void rangeFormatting(const newlsp::DocumentRangeFormattingParams &params){}
    // textDocument/onTypeFormatting
    void onTypeFormatting(const newlsp::DocumentOnTypeFormattingParams &params){}
    // textDocument/rename
    void rename(const newlsp::RenameParams &params){}
    // textDocument/prepareRename
    void prepareRename(const newlsp::PrepareRenameParams &params){}
    // textDocument/linkedEditingRange
    void linkedEditingRange(const newlsp::LinkedEditingRangeParams &params){}
    // textDocument/publishDiagnostics
    void publishDiagnostics(const newlsp::PublishDiagnosticsParams &params);

    // codeAction/resolve
    void resolve(const newlsp::CodeAction &codeAction){}
    // completionItem/resolve
    void resolve(const newlsp::CompletionItem &params){}
    // inlayHint/resolve
    void resolve(){}
    // codeLens/resolve
    void resolve(const newlsp::CodeLens &codeLens){}

    // typeHierarchy/supertypes
    void supertypes(const newlsp::TypeHierarchySupertypesParams &params){}
    // typeHierarchy/subtypes
    void subtypes(const newlsp::TypeHierarchySubtypesParams &params){}

    // callHierarchy/incomingCalls
    void incomingCalls(const newlsp::CallHierarchyIncomingCallsParams &params){}
    // callHierarchy/outgoingCalls
    void outgoingCalls(const newlsp::CallHierarchyOutgoingCallsParams &params){}

    // workspace/codelens/refresh
    void refresh(){}

    // workspace/semanticTokens/refresh
    void workspace_semanticTokens_refresh(){}

    // workspace/inlayHint/refresh
    void workspace_inlayHint_refresh(){}

    // workspace/inlineValue/refresh
    void workspace_inlineValue_refresh(){}

    // workspace/diagnostic/refresh
    void workspace_diagnostic_refresh(){}

    // workspace/diagnostic
    void diagnostic(const newlsp::WorkspaceDiagnosticParams &params){}

public slots:
    void setHead(const lsp::Head &head);
    void initRequest(const QString &compile); // yes
    void shutdownRequest();
    void exitRequest();
    void openRequest(const QString &filePath); // yes
    void closeRequest(const QString &filePath); // yes
    void changeRequest(const QString &filePath, const QByteArray &text); // yes
    void symbolRequest(const QString &filePath); // yes
    void renameRequest(const QString &filePath, const lsp::Position &pos, const QString &newName); //yes
    void definitionRequest(const QString &filePath, const lsp::Position &pos); // yes
    void completionRequest(const QString &filePath, const lsp::Position &pos); // yes
    void signatureHelpRequest(const QString &filePath, const lsp::Position &pos); // yes
    void referencesRequest(const QString &filePath, const lsp::Position &pos);
    void docHighlightRequest(const QString &filePath, const lsp::Position &pos);
    void docSemanticTokensFull(const QString &filePath); //yes
    void docHoverRequest(const QString &filePath, const lsp::Position &pos); // yes
    void processJson(const QJsonObject &jsonObj);

signals:
    void request();
    void notification(const QString &jsonStr);
    void notification(const lsp::DiagnosticsParams &diagnostics);
    void requestResult(const lsp::SemanticTokensProvider &tokensProvider);
    void requestResult(const lsp::Symbols &symbols);
    void requestResult(const lsp::Locations &locations);
    void requestResult(const lsp::CompletionProvider &completionProvider);
    void requestResult(const lsp::SignatureHelps &signatureHelps);
    void requestResult(const lsp::Hover &hover);
    void requestResult(const lsp::Highlights &highlights);
    void requestResult(const lsp::DefinitionProvider &definitionProvider);
    void requestResult(const QList<lsp::Data> &tokensResult);
    void requestResult(const newlsp::WorkspaceEdit &changes);
    void requestResult(const lsp::References &refs);

private:
    bool calledError(const QJsonObject &jsonObj);
    bool calledResult(const QJsonObject &jsonObj); //found result key from json && not found method
    bool initResult(const QJsonObject &jsonObj); // client call server rpc return
    bool openResult(const QJsonObject &jsonObj); // client call server rpc return
    bool changeResult(const QJsonObject &jsonObj); // client call server rpc return
    bool symbolResult(const QJsonObject &jsonObj); // client call server rpc return
    bool renameResult(const QJsonObject &jsonObj); // client call server rpc return
    bool definitionResult(const QJsonObject &jsonObj); // client call server rpc return above uri
    bool completionResult(const QJsonObject &jsonObj); // client call server rpc return
    bool signatureHelpResult(const QJsonObject &jsonObj); // client call server rpc return
    bool hoverResult(const QJsonObject &jsonObj); // client call server rpc return
    bool referencesResult(const QJsonObject &jsonObj); // client call server rpc return
    bool docHighlightResult(const QJsonObject &jsonObj); // client call server rpc return
    bool docSemanticTokensFullResult(const QJsonObject &jsonObj); // client call server rpc return
    bool closeResult(const QJsonObject &jsonObj); // client call server rpc return
    bool shutdownResult(const QJsonObject &jsonObj);
    bool exitResult(const QJsonObject &jsonObj);

    bool serverCalled(const QJsonObject &jsonObj); // not found result key from json && found key method
    bool diagnostics(const QJsonObject &jsonObj);

private slots:
    QStringList cvtStringList(const QJsonArray &array);
};
} // namespace lsp

#endif // CLIENT_H
